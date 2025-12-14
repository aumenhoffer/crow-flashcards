function addCard()
{
    var cards = document.getElementById("cards");
    var card = document.createElement('div');
    card.className = "card";
    card.innerHTML =
    `
    <input type="text" class="term">
    <input type="text" class="definition">
    `
    var deleteButton = document.createElement("button");
    deleteButton.addEventListener('click', function() {
        card.remove();
    })
    deleteButton.className = 'danger';
    deleteButton.innerText = `Delete`;
    card.appendChild(deleteButton);
    cards.appendChild(card)
}

function submitSet()
{
    body = {
        name: document.getElementById("name").value,
        auth: document.getElementById("author").value,
        desc: document.getElementById("description").value,
        cards: []
    }
    var cards = document.getElementById("cards")
    for(let i = 0; i < cards.children.length; i++) {
        if(cards.children[i].className != "card") return;
        body.cards.push({
            t: cards.children[i].children[0].value,
            d: cards.children[i].children[1].value
        })
    }

    console.log(body)
    
    fetch("/sets", {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify(body)
    })
    .then(res => {
        if(res.status == 200) {
            window.location.href="/"
        }
        else { alert("Failed to create set!") }
    })


}