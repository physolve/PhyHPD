import QtQuick
Item { 
    property url source
    property var jsonObject
    function load(url) {
        // console.log(url)
        let xhr = new XMLHttpRequest();
        xhr.onreadystatechange = function() {
            // console.log(xhr.readyState)
            if (xhr.readyState !== XMLHttpRequest.DONE) {
                return;
            }
            // console.log(xhr.response.toString())
            // console.log(xhr.responseText.toString())
            jsonObject = JSON.parse(xhr.responseText.toString());
        }
        // xhr.responseType = "json";
        xhr.open("GET", url); 
        xhr.send();
    }
    function save() {
        const blob = new Blob([JSON.stringify(jsonObject, null, 2)], {
            type: 'application/json',
        });
        const url = URL.createObjectURL(blob);
        const a = document.createElement('a');
        a.href = url;
        a.download = `test.json`;
        a.click();
        URL.revokeObjectURL(url);
    }
    
    Component.onCompleted: {
        load(source)
    }
}