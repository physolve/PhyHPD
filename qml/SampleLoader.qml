import QtQuick
Item { 
    property url source
    property var jsonObject
    function load(url) {
        console.log(url)
        let xhr = new XMLHttpRequest();
        xhr.onreadystatechange = function() {
            console.log(xhr.readyState)
            if (xhr.readyState !== XMLHttpRequest.DONE) {
                return;
            }
            console.log(xhr.response.toString())
            console.log(xhr.responseText.toString())
            jsonObject = JSON.parse(xhr.responseText.toString());
        }
        // xhr.responseType = "json";
        xhr.open("GET", url); 
        xhr.send();
    }
    Component.onCompleted: {
        load(source)
    }
}