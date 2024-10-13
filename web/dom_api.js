const evtSource = new EventSource("https://crispy-cod-wr54vggr7xfgvpg-8000.app.github.dev/sse");
const appendData = document.getElementById("sseData");

let sseCommands = [];

evtSource.onmessage = (event) => {
    // TODO: trim numerous dom updates that occur in <500ms
    dirty_parse(event.data);
    appendData.innerHTML += `[${event.timeStamp.toFixed(2)}] ${event.data}<br>`;
};

function dirty_parse(dom_update) {
    const tokens = dom_update.split(" ");
    
    for (let i = 0; i < tokens.length; i++) {
        const token = tokens[i];
        switch (token) {
            // update $id value computed?
            case "update":
                const id = tokens[i + 1];
                const value = tokens[i + 2];
                const computed = tokens[i + 3];

                if (computed != "null") {
                    console.warn("not implemented");
                    console.error("computed dom updates are not safe");
                    return;
                }
                
                document.getElementById(id).innerHTML = value;
                i += 3;
        }
    }
}