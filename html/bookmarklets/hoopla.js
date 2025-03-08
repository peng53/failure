document.querySelectorAll(".absolute.h-full")

javascript:(()=>{
if (document.URL.startsWith("https://www.hoopladigital.com")){
Array.from(document.querySelectorAll(".from-black")).forEach(e=>{
e.style.background='none';
});
Array.from(document.querySelectorAll(".w-32")).forEach(e=>{
e.style.width='1rem';
});
}
})();