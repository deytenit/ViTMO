window.notify = (message) => {
    $.notify(message, {
        position: "right bottom",
        className: "success"
    });
}

window.ajax = (request, callback) => {
    $.ajax({
        type: "POST",
        url: "",
        data: request,
        dataType: "json",
        encode: true,
    }).done((response) => {
        if (response.redirect) {
            location.href = response.redirect;
        } else if (callback) {
            callback(response);
        }
    });
}
