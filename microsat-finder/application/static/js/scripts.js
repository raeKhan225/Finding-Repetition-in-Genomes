var upload_confirm_form_listener = document.getElementById("upload_confirm_form");

if (upload_confirm_form_listener){
upload_confirm_form_listener.addEventListener("submit", function(e) {
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
};

var view_current_jobs_listener = document.getElementById("view_current_jobs");
if (view_current_jobs_listener) {

  document.getElementById("view_current_jobs").addEventListener("submit", function(e) {
    var $form = $(document.getElementById("view_current_jobs"));
    var $email = $form.serialize();
    console.log("HERE");
    $.ajax({
      url: "/view_current_jobs/",
      type: "POST",
      data: $email,
      dataType: "json",
      error: function(resp) {
        console.log(resp);
        alert("Error: " + resp);
      },
      success: function(resp) {
      e.preventDefault();
      console.log(resp);
      // Replace with success message
      console.log("Redirecting to /");
      window.location.href = "/";
      }
    });
    e.preventDefault(); // stop form from submitting
  });
};




function current_jobs_func() {
  document.getElementById("current-jobs-modal").style.display = "block";
};

function close_modal() {
  document.getElementById("current-jobs-modal").style.display = "none";
};





