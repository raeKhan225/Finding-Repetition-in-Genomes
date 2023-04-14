$("form[name=upload_confirm_form").submit(function(e) {

  var $form = $(this);
  var $error = $form.find(".error");
  var data = $form.serialize();

  $.ajax({
    url: "/upload/file/",
    type: "POST",
    data: data,
    dataType: "json",
    success: function(resp) {
      console.log(resp);
    },
    error: function(resp) {
     console.log(resp);;
    }
  });

  e.preventDefault();
});
