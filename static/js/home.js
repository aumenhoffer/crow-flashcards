function sanitizeHTML(str) {
    console.log(str)
  return str.replace(/&/g, "&amp;").replace(/</g, "&lt;")
  .replace(/>/g, "&gt;").replace(/"/g, "&quot;").replace(/'/g, "&#39;");
}


document.addEventListener("DOMContentLoaded", async () => {
    const sets = document.getElementById("sets");
    res = await fetch("/sets/raw", {
        method: "GET"
    })
    if(res.status != 200) return alert("Failed to fetch sets!");
    data = await res.json();

    console.log(data)

    for(let i = 0; i < data.length; i++)
    {
        var set = document.createElement("div");
        let l = data[i].cards.length
        set.className = "set";
        set.innerHTML = `
            <h2>${sanitizeHTML(data[i].name)}</h2>
            <p>By: <span>${sanitizeHTML(data[i].auth)}</span></p>
            <small>There ${(l>1||l==0)?"are":"is"} <strong class="length">${data[i].cards.length}</strong> card${(l>1||l==0)?"s":""} in this set</small>
            <p>Description: ${sanitizeHTML(data[i].desc)}</p>
        `
        set.addEventListener('click', function() {
            window.location.href=`/sets/${data[i].id}`
        })
        sets.appendChild(set);
    }
})