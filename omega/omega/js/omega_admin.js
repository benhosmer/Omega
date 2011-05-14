(function ($) {
  Drupal.behaviors.omegaAdmin = {
    attach: function (context) {
      // hide all collapsible fieldset stuffs
      $('.omega-accordion-content').hide();
      // show the first fieldset in any group of "accordion" items
      firsts = $([]);
      $('.fieldset-wrapper').each(function(){
        var new_first = $(this).children('.omega-accordion:first');
        firsts = firsts.add(new_first);
      });  
      firsts
        .children('a')
          .addClass('expanded')
          .end() // return to the h3
        .next('.omega-accordion-content')
        .addClass('expanded')
        .show();
      // provide click/toggle functionality
      $('.omega-accordion a').click(function(){
        // remove expanded class from all href items
        $(this).parents('.fieldset-wrapper').find('a').removeClass('expanded');
        // add expanded back to the cliked href
        $(this).addClass('expanded');
        var clicked = $(this).parent('h3');
        // if we click a header that is already open, do nothing
        if(clicked.next('.omega-accordion-content').hasClass('expanded')) {
          return false;
        }
        else {
          clicked
            .next('.omega-accordion-content')
              .slideDown('fast')
              .addClass('expanded')
              .addClass('active-accordion')
              .end()
            .parents('.fieldset-wrapper')
            .children('.omega-accordion-content:not(.active-accordion)')
            .removeClass('expanded')
            .slideUp('fast');
          $('.omega-accordion-content').removeClass('active-accordion');
          return false;
        }
      });
      
      // hide the sidebar on theme settings page
      //$('.column-side').css('display', 'none');
      // make the main column full width
      //$('.column-main').css('width', '100%');
      // remove bg image
      //$('.form-layout-default').css('background', 'none');
    }
  };
})(jQuery);
