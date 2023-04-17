$("form[name=upload_confirm_form").submit(function(e) {

  var $form = $(this);
  var $error = $form.find(".error");
  var $success = $form.find(".success");
  var data = $form.serialize();

  $.ajax({
    url: "/upload/file/",
    type: "POST",
    data: data,
    dataType: "json",
    success: function(resp) {
      console.log(resp);
      // Replace with success message
      window.location.href = "/";

    },
    error: function(resp) {
     console.log(resp);
    }
  });

  e.preventDefault();
});

function current_jobs_func() {

    document.getElementById("current-jobs-modal").style.display = "block" ;
};

function close_modal(){
    document.getElementById("current-jobs-modal").style.display = "none" ;
};


$(document).on("submit", "form[name=view_current_jobs]", function(e) {
    var $form = $(this);
    var data = $form.serialize();

    $.ajax({
        url: "/view_current_jobs/",
        type: "POST",
        data: data,
        dataType: "json",
        success: function(resp) {
            console.log(resp);
            close_modal(); // Call the close_modal function after 1 second
        },

        error: function(resp) {
            console.log(resp);
        }
    });

    // Prevent the form from submitting normally
    e.preventDefault();
});

