// Finds upload form element
var upload_confirm_form_listener = document.getElementById("upload_confirm_form");
// Adds a listener if there to prevent adding null to listener
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
          document.getElementById("showResponseArea").removeAttribute("hidden");
          document.getElementById("upload_confirm_form").reset();

        },
        error: function(resp) {
          console.log( resp);
          alert("Error: the file upload failed, please try again");
       }
    });
    //stop form from submitting
    e.preventDefault();
});
};

var view_current_jobs_listener = document.getElementById("view_current_jobs");
if (view_current_jobs_listener) {
  document.getElementById("view_current_jobs").addEventListener("submit", function(e) {
    var $form = $(document.getElementById("view_current_jobs"));
    var $email = $form.serialize();

    $.ajax({
      url: "/view_current_jobs/",
      type: "POST",
      data: $email,
      dataType: "json",
      error: function(resp) {
        console.log(resp);
        close_modal();
        alert("Error: your jobs can't be retrieved, please try again");
      },
      success: function(resp) {
          console.log(resp);
          // Replace with success message
          console.log("Redirecting to /");
          close_modal();
          document.getElementById("showResponseArea_curr_jobs").removeAttribute("hidden");
      }
    });
    // stop form from submitting when opened
    e.preventDefault();
    // stop form from submitting twice when submit button is pressed
    e.stopImmediatePropagation();
  });
};


// Opens pop-up block
function current_jobs_func() {
  document.getElementById("current-jobs-modal").style.display = "block";
};

// Closes pop-up block
function close_modal() {
  document.getElementById("current-jobs-modal").style.display = "none";
};





