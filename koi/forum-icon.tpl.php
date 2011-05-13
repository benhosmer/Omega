<?php
// $Id: forum-icon.tpl.php,v 1.4 2010/12/29 16:32:54 jarek Exp $

/**
 * @file
 * Default theme implementation to display an appropriate icon for a forum post.
 *
 * Available variables:
 * - $new_posts: Indicates whether or not the topic contains new posts.
 * - $icon: The icon to display. May be one of 'hot', 'hot-new', 'new',
 *   'default', 'closed', or 'sticky'.
 *
 * @see template_preprocess_forum_icon()
 * @see theme_forum_icon()
 */
?>
<div class="topic-status-<?php print $icon_class ?>" title="<?php print $icon_title ?>">
<?php if ($first_new): ?>
  <a id="new"></a>
<?php endif; ?>

  <span class="element-invisible"><?php print $icon_title ?></span>

</div>
