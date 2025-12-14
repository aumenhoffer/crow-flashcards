var set = {};
var shuffle = [];

function sanitizeHTML(str) {
  return str .replace(/&/g, "&amp;").replace(/</g, "&lt;")
  .replace(/>/g, "&gt;").replace(/"/g, "&quot;").replace(/'/g, "&#39;");
}

function loadSet()
{
    var title = document.getElementById("title");
    var author = document.getElementById("author");
    var description = document.getElementById("description");
    var length = document.getElementsByClassName("length");
    var cards = document.getElementById("cards");
    title.innerText = set.name;
    author.innerText = set.auth;
    description.innerText = set.desc?set.desc:"No description given";
    for(e of length) { e.innerText = set.cards.length; }

    for(let i = 0; i < set.cards.length; i++) {
        var card = document.createElement("div");
        card.innerHTML = `
        <div>
        <span class="term">${sanitizeHTML(set.cards[i].t)}</span> - 
        <span class="definition">${sanitizeHTML(set.cards[i].d)}</span>
        </div>
        `;
        cards.appendChild(card);
    }
}

document.addEventListener("DOMContentLoaded", async () => {
    var id = window.location.pathname.replace(/\/$/, "").split("/").pop();
    var res = await fetch("/sets/"+id+"/raw", {
        method: "GET"
    });
    if(res.status != 200) return alert("No such set exists!")
    data = await res.json();

    set = data; 
    loadSet();

    document.getElementById("delete_button").addEventListener('click', function() {
        window.location.href=`/sets/${set.id}/delete`
    });

    shuffle = [...Array(set.cards.length).keys()];
    const a = document.getElementById("practice_a");
    const q = document.getElementById("practice_q");
    const r = document.getElementById("restart");
    const stop = document.getElementById("stop");
    const holder = document.getElementById("cards");
    let i = 0;
    let c = 0;
    const p = document.getElementById("practice")
    var wrong = true;

    if(set.cards.length < 1) 
    {
        p.style.display = "block";
        p.innerHTML = `<p>You can't practice an empty set!</p>`
        r.style.display = "none";
        return;
    };

    function endPractice()
    {
        a.style.display = "none";
        r.style.display = "block"
        q.innerText = "Practice Complete!";
        stop.style.display = "none";
        holder.classList.remove("blur");
    }

    function reset()
    {
        i = 0;
        c = 0;
        wrong = false;
        shuffleArray();
        document.getElementById("practice_q").innerText = set.cards[shuffle[i]].t;
        document.getElementById("correct").innerText = 0;
        document.getElementById("incorrect").innerText = 0;
        document.getElementById("index").innerText = 0;
        p.classList.remove("fadeRed");
        p.classList.remove("fadeGreen");
        a.style.display = "block";
        stop.style.display = "block";
        r.style.display = "none"
        holder.classList.add("blur");
        p.style.display = "block";
    }

    function nextQuestion()
    {
        if(a.value.toLowerCase() == set.cards[shuffle[i]].d.toLowerCase()) {
            if(!wrong) c++;
            else wrong = false;
            p.classList.add("fadeGreen");
        }
        else {
            a.value = "";
            p.classList.add("fadeRed");
            a.placeholder = "Try again!";
            wrong = true;
            return;
        }

        a.value = "";

        document.getElementById("correct").innerText = c;
        document.getElementById("incorrect").innerText = i-c+1;

        if(i >= set.cards.length-1)
        {
            endPractice();
            return;
        }

        i++;
        q.innerText = set.cards[shuffle[i]].t;
        document.getElementById("index").innerText = i+1;
        a.placeholder = "Type your answer here";
    }

    r.addEventListener("click", () => { reset(); });
    stop.addEventListener("click", () => { endPractice(); });

    p.addEventListener('submit', (event) => {
        event.preventDefault();
        document.getElementById("practice_a").focus();
        p.classList.remove("fadeRed");
        p.classList.remove("fadeGreen");
        setTimeout(() => {
            nextQuestion();
        }, 10);
    })

})

function shuffleArray()
{
    let i = set.cards.length;
    while(i != 0)
    {
        let r = Math.floor(Math.random() * i);
        i--;
        var t = shuffle[i];
        shuffle[i] = shuffle[r];
        shuffle[r] = t;
    }
}