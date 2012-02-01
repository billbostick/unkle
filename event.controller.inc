<?php
/**
 * @file
 * default entity controller
 */
class EventController extends DrupalDefaultEntityController {

  public function save($event) {
    global $user;
    $event->uid = $user->uid;
    $event->timestamp = time();
    field_attach_presave('unkle_event', $event);
    if (!empty($event->event_id)) {
      drupal_write_record('unkle_event', $event, 'event_id');
      field_attach_update('unkle_event', $event);
      module_invoke_all('entity_update', 'unkle_event', $event);
      return $event;
    }
    drupal_write_record('unkle_event', $event);
    field_attach_insert('unkle_event', $event);
    module_invoke_all('entity_insert', 'unkle_event', $event);
    return $event;
  }
}

