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

/*$("#current-jobs-btn").click(function(e){
    $("#current-jobs-modal").modal('show');
});*/

$("form[name=view_current_jobs").submit(function(e) {

  var $form = $(this);
  var $email = $form.serialize();

  $.ajax({
    url: "/view_current_job/",
    type: "POST",
    data: email,
    dataType: "json",
    success: function(resp) {
      console.log(resp);
      // Replace with success message
      window.location.href = "/upload";

    },
    error: function(resp) {
     console.log(resp);
    }
  });

  e.preventDefault();
});

