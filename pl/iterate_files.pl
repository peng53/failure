# perl generate jsongrid data

opendir(DH,".");
my @files = readdir(DH);
closedir(DH);

$len = @files-2;
print "files count: $len\n";
print "[";
my $json = '';
my $run = 0;
foreach my $file (@files){
	next if ($file =~ /^\.{1,2}$/);
	if ($run != 0){
		print ',';
	}
	$json = qq({"name": "$file", "thumb":"$file.png", "tag":[]}\n);
	print $json;
	$run++;
}
print "]\n";
