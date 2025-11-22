add-type -assemblyname system.collections

<#
# if using powershell 7.3
#----
add-type -assemblyname system.text.json

$left = [system.text.json.jsondocument]::parse((get-content ..\in_out\testjson-diff-a.json))
$right = [system.text.json.jsondocument]::parse((get-content ..\in_out\testjson-diff-b.json))

#>
$left = get-content ..\in_out\testjson-diff-a.json | convertfrom-json
$right = get-content ..\in_out\testjson-diff-b.json | convertfrom-json

$all_keys = [system.collections.generic.hashset[string]]::new()

#get keys that intersect, alert on those that don't (right)
foreach ($k in $left.psobject.properties.name){
	if ($right.psobject.properties.name -contains $k){
		[void]$all_keys.add($k)
	} else {
		write-host ("Missing key on right: root.{0}" -f $k)
	}
}
#alert on missing left
foreach ($k in $right.psobject.properties.name){
	if ($left.psobject.properties.name -contains $k){
		continue
	} else {
		write-host ("Missing key on left: root.{0}" -f $k)
	}
}

$stk = [system.collections.stack]::new()
#add keys to stack
foreach ($k in $all_keys){
	$stk.push(@{
		key='root.'+$k;
		l=$left.$k;
		r=$right.$k;
		i=0
	})
}
$all_keys.clear()
# initial setup done, now to iterate on stk

while ($stk.count -gt 0){
	$item = $stk.pop()
	$ltype = $item.l.gettype().basetype.name
	$rtype = $item.r.gettype().basetype.name
	if ($ltype -ne $rtype){
		write-host 'Mismatched key left and right:' $item.key
		continue
	}
	#write-host $item.l.gettype().name
	switch ($item.l.gettype().name){
		'PSCustomObject' {
			foreach ($sk in $item.l.psobject.properties.name){
				if ($item.r.psobject.properties.name -contains $sk){
					[void]$all_keys.add($sk)
				} else {
					write-host ('  '*$item.i) -nonewline
					write-host ('Missing key on right: {0}.{1}' -f ($item.key,$sk))
				}
			}
			foreach ($sk in $item.r.psobject.properties.name){
				if ($item.l.psobject.properties.name -contains $sk){
					continue
				} else {
					write-host ('  '*$item.i) -nonewline
					write-host ('Missing key on left: {0}.{1}' -f ($item.key,$sk))
				}
			}
			foreach ($k in $all_keys){
				$stk.push(@{
					key=('{0}.{1}' -f ($item.key,$k));
					l=$item.l.$k;
					r=$item.r.$k;
					i=$item.i+1;
				})
			}
			$all_keys.clear()
		}
		'Object[]' {
			$m = $item.l.count
			if ($item.l.count -ne $item.r.count){
				write-host ('  '*$item.i) -nonewline
				write-host ('Array size discrepency at key {0}; {1} and {2}' -f ($item.key,$item.l.count,$item.r.count))
				$m = [math]::min($item.l.count,$item.r.count)
			}
			for ($i=0; $i -lt $m; ++$i){
				$stk.push(@{
					key=('{0}[{1}]' -f($item.key,$i));
					l=$item.l[$i];
					r=$item.r[$i];
					i=$item.i+1;
				})
			}
		}
		default {
			if ($item.l -ne $item.r){
				write-host ('  '*$item.i) -nonewline
				write-host ('Different value at key: {0}; {1} =!= {2}' -f ($item.key,$item.l,$item.r))
			}
		}
	}
}
