// Line Chart Colors
var linecolors = [ "#727272", "#f1595f", "#79c36a", "#599ad3", "#f9a65a", "#9e66ab", "#cd7058", "#d77fb3" ];

$(document).ready(function()
{
});

$.dynatableSetup({
    features:
    {
        search: false,
        recordCount: false,
        perPageSelect: false,
        pushState: false,
        paginate: false
    }
});

$.getJSON("Log.json", function(data)
{
    console.log(data);
    
    $('#datatable').dynatable({
        dataset:
        {
            records: data.players
        }
    });
    
    createPlayerSection(data);
    
    Morris.Line({
        element: "linechart",
        data: data.rounds,
        xkey: "r",
        ykeys: data.ykeys,
        labels: data.labels,
        ymax: 1,
        ymin: 5,
        smooth: false,
        hideHover: true,
        parseTime: false,
        axes: true,
        resize: true,
        yLabelFormat: formatRole,
        lineColors: linecolors
    });
    
    achievements(data);
    appendTable(data);
    
    /*$('#illegaltable').dynatable({
        dataset:
        {
            records: data.error_Log
        }
    });*/
});

function createPlayerSection(data)
{
    var section = document.getElementById("playercolors");
    
    for(var i = 0; i < data.labels.length; i++)
    {
        var para = document.createElement("span");
        para.style.backgroundColor = linecolors[i];
        
        var node = document.createTextNode(data.labels[i]);
        para.appendChild(node);
        
        section.appendChild(para);
    }
}

function achievements(data)
{
    var king = getHighest(data, "greater_Dalmuti");
    setAchievement("king_a", "king_count", king.name, king.value, king.index)
    
    var merchant = getHighest(data, "merchant");
    setAchievement("merchant_a", "merchant_count", merchant.name, merchant.value, merchant.index)
    
    var peon = getHighest(data, "greater_Peon");
    setAchievement("peon_a", "peon_count", peon.name, peon.value, peon.index)
    
    var fast = getLowest(data, "average_Turntime");
    setAchievement("fast_a", "fast_count", fast.name, fast.value, fast.index)
    
    var slow = getHighest(data, "average_Turntime");
    setAchievement("slow_a", "slow_count", slow.name, slow.value, slow.index)
    
    var cheat = getHighest(data, "illegal_Moves");
    if(cheat.value <= 0)
    {
        var ele = document.getElementById("cheat_text");
        ele.innerHTML = "Amazingly nobody was caught cheating during this game."
    }
    else
    {
        setAchievement("cheat_a", "cheat_count", cheat.name, cheat.value, cheat.index)
    }
}

function getHighest(data, search)
{
    var nam = data.players[0].name;
    var val = data.players[0][search];
    var id = 0;
    
    for(var i = 1; i < data.players.length; i++)
    {
        if(data.players[i][search] > val)
        {
            nam = data.players[i].name;
            val = data.players[i][search];
            id = i;
        }
    }

    return { "name": nam, "value": val, "index": id };
}

function getLowest(data, search)
{
    var nam = data.players[0].name;
    var val = data.players[0][search];
    var id = 0;
    
    for(var i = 1; i < data.players.length; i++)
    {
        if(data.players[i][search] < val)
        {
            nam = data.players[i].name;
            val = data.players[i][search];
            id = i;
        }
    }

    return { "name": nam, "value": val, "index": id };
}

function setAchievement(achievement, count, name, value, index)
{
    var ele = document.getElementById(achievement);
    ele.innerHTML = name;
    ele.style.backgroundColor = linecolors[index];
    document.getElementById(count).innerHTML = value;
}

function formatNames(names)
{
    if(names.length == 1) // Single player.
    {
        return names[0];
    }
    else if(names.length == 2) // Two players.
    {
        return names[0] + " and " + names[1];
    }

    var n = names[0];
    for(var i = 1; i < names.length - 1; i++) // More than two players.
    {
        n += (", " + names[i]);
    }
    n += " and " + names[names.length - 1];
    return n;
}

function formatRole(data)
{
    var role;
    switch(data)
    {
        case 1:
            role = "Greater Dalmuti";
            break;
        case 2:
            role = "Lesser Dalmuti";
            break;
        case 3:
            role = "Merchant";
            break;
        case 4:
            role = "Lesser Peon";
            break;
        case 5:
            role = "Greater Peon";
            break;
    }
    return role;
}

function appendTable(data)
{
    var table = document.getElementById("illegaltable");
    for(var i = 0; i < data.error_Log.length; i++)
    {
        var tr = table.insertRow(i + 1);
        var c1 = tr.insertCell(0);
        c1.innerHTML = data.error_Log[i].name;
        
        var c2 = tr.insertCell(1);
        formatMove(c2, data.error_Log[i].move);
        
        var c3 = tr.insertCell(2);
        formatMove(c3, data.error_Log[i].latest_Table);
        
        var c4 = tr.insertCell(3);
        formatHand(c4, data.error_Log[i].hand_Cards);
    }
}

function formatMove(cell, move)
{
    for(var i = 0; i < move.amount; i++)
    {
        var img = document.createElement("img");
        img.src = "pics/c_" + move.value + ".png";
        img.alt = move.value;
        cell.appendChild(img);
    }
    
    for(var j = 0; j < move.jesters; j++)
    {
        var img = document.createElement("img");
        img.src = "pics/c_13.png";
        img.alt = "J";
        cell.appendChild(img);
    }
}

function formatHand(cell, hand)
{
    for(var i = 0; i < hand.length; i++)
    {
        var img = document.createElement("img");
        img.src = "pics/c_" + hand[i] + ".png";
        img.alt = hand[i];
        cell.appendChild(img);
    }
}