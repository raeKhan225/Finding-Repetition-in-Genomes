document.getElementById("upload_confirm_form").addEventListener("submit", function(e) {
    var $form = $(this);
    var form_data = new FormData($form[0]);
    var file_input = document.getElementById("fastaFile");
    form_data.append("fastaFile", file_input.files[0]);

    $.ajax({
        url: "/upload_file/",
        type: "POST",
        data: form_data,
        cache: false,
        //dataType: "multi",
        contentType: false,
        processData: false,

        success: function(resp) {
          console.log(resp);
          // Replace with success message
          console.log("Redirecting to /");
          window.location.href = "/";

        },
        error: function(resp) {
          console.log( resp);
          alert("Error: " + resp)
       }
    });
    e.preventDefault();    //stop form from submitting
});

function get_current_jobs_form() {

    var $form = $(this);
    var data = new FormData($form[0]);

    $.ajax({
        url: "/upload_file/",
        type: "POST",
        data: data,
        cache: false,
        contentType: false,
        processData: false,

        success: function(resp) {
          console.log(resp);
          // Replace with success message
          console.log("Redirecting to /");
          window.location.href = "/";

        },
        error: function(resp) {
          console.log( resp);
          alert("Error: " + resp)
       }
    });
    e.preventDefault();    //stop form from submitting

};


function current_jobs_func() {
  document.getElementById("current-jobs-modal").style.display = "block";
};

function close_modal() {
  document.getElementById("current-jobs-modal").style.display = "none";
};





