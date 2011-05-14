(function ($) {
  Drupal.behaviors.manipulateFormElements = {
    attach: function(context, settings) {
    // give the login form some love
    $('#user-login-form .login-submit-link').click(function(){
    	$('#user-login-form').submit();
    	return false;
    });
    }
  };
  Drupal.behaviors.correctActiveTrails = {
    attach: function(context, settings) {
      // fix menus that don't respect active trail because drupal links are stoopid
    $('#region-menu ul li.active').parents('li').addClass('active-trail');
    }
  };
})(jQuery);
