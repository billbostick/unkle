<?php

/**
 * @file
 * utility function for the unkle module
 */

/*
 * trying to restrict all of the "$node->property['und'][0]['value']" type references to this file
 
function unkle_util_get_language($node)
  
function unkle_util_get_person_title($person)
function unkle_util_get_person_title_nodelink($person)
function unkle_util_get_person_fullname_nodelink($person)
function unkle_util_get_person_title_treelink($person)
function unkle_util_get_person_treelink($person)
function unkle_util_get_person_birthdate($person)
function unkle_util_get_person_given_name($person)
function unkle_util_get_person_surname($person)
function unkle_util_get_person_fullname($person)
function unkle_util_get_person_childtofamily_nid($person)

function unkle_util_get_family_title($family)
function unkle_util_get_family_husband_nid($family)
function unkle_util_get_family_wife_nid($family)
function unkle_util_get_family_children($family)
  
  
function unkle_util_get_siblings($person)
function unkle_util_get_families_as_spouse($person)
function unkle_util_get_spouse_for_family($person, $family)
function unkle_util_get_spouses($person)
function unkle_util_get_half_siblings($person)
function unkle_util_get_spouse_title($spouse_id)

function unkle_util_show_spouses_and_children($person)

 */

function unkle_util_get_person_title($person) {
  return $person->title;
}

function unkle_util_get_person_title_nodelink($person) {
  return  '<a href="/node/' . $person->nid . '">' . $person->title . '</a>';
}

function unkle_util_get_person_fullname_nodelink($person) {
  $fullname = unkle_util_get_person_fullname($person);
  return  '<a href="/node/' . $person->nid . '">' . $fullname . '</a>';
}

function unkle_util_get_person_title_treelink($person) {
  return  '<a href="/node/' . $person->nid . '/tree">' . $person->title . '</a>';
}

function unkle_util_get_person_fullname_treelink($person) {
  $fullname = unkle_util_get_person_fullname($person);
  return  '<a href="/node/' . $person->nid . '/tree">' . $fullname . '</a>';
}

function unkle_util_get_person_treelink($person) {
}

function unkle_util_get_person_birthdate($person) {
}

function unkle_util_get_person_given_name($person) {
  $lang = unkle_util_get_language($person);
  if (property_exists($person, 'person_given_name') &&
      is_array($person->person_given_name) &&
      count($person->person_given_name) &&
      $person->person_given_name[$lang][0]['value']) {
    return $person->person_given_name[$lang][0]['value'];
  }
  else  {
    return 'Unknown';
  }
}

function unkle_util_get_person_surname($person) {
  $lang = unkle_util_get_language($person);
  if (property_exists($person, 'person_surname') &&
      is_array($person->person_surname) &&
      count($person->person_surname) &&
      $person->person_surname[$lang][0]['value']) {
    return $person->person_surname[$lang][0]['value'];
  }
  else  {
    return 'Unknown';
  }
}

function unkle_util_get_person_fullname($person) {
  $fullname = '';
  $surname = unkle_util_get_person_surname($person);
  $given_name = unkle_util_get_person_given_name($person);
  if (($surname == 'Unknown') && ($given_name == 'Unknown')) {
    $fullname = 'Unknown';
  }
  else {
    $fullname = $surname . ', ' . $given_name;
  }
  return $fullname;
}

function unkle_util_get_person_childtofamily_nid($person) {
  $lang = unkle_util_get_language($person);
  if (property_exists($person, 'person_childtofamily') &&
      is_array($person->person_childtofamily) &&
      count($person->person_childtofamily) &&
      $person->person_childtofamily[$lang][0]['target_id']) {
    return $person->person_childtofamily[$lang][0]['target_id'];
  }
  else  {
    return NULL;
  }
}

function unkle_util_get_family_title($family) {
  return $family->title;
}

function unkle_util_get_family_husband_nid($family) {
  $lang = unkle_util_get_language($family);
  if (property_exists($family, 'family_husband') &&
      is_array($family->family_husband) &&
      count($family->family_husband) &&
      $family->family_husband[$lang][0]['target_id']) {
    return $family->family_husband[$lang][0]['target_id'];
  }
  else  {
    return NULL;
  }
}

function unkle_util_get_family_wife_nid($family) {
  $lang = unkle_util_get_language($family);
  if (property_exists($family, 'family_wife') &&
      is_array($family->family_wife) &&
      count($family->family_wife) &&
      $family->family_wife[$lang][0]['target_id']) {
    return $family->family_wife[$lang][0]['target_id'];
  }
  else  {
    return NULL;
  }
}

function unkle_util_get_family_children($family) {
  if ($family) {
    $lang = unkle_util_get_language($family);
    if (property_exists($family, 'family_child') &&
        is_array($family->family_child) &&
        count($family->family_child) &&
        count($family->family_wife[$lang])) {
      foreach ($family->family_child[$lang] as $child) {
        $children[] = $child['target_id'];
      }    
      return $children;
    }
    else  {
      return NULL;
    }
  }
}

function unkle_util_get_siblings($person) {
  $siblings = array();
  $family = unkle_util_get_person_childtofamily_nid($person);
  $fam_children = unkle_util_get_family_children(node_load($family));
  if ($fam_children) {
    foreach ($fam_children as $child) {
      if ($child != $person->nid) {
        $siblings[] = $child;
      }
    }
  }
  if (count($siblings)) {
    return $siblings;
  }
  return NULL;
}

function unkle_util_get_language($node) {
  if ($node) {
    if (property_exists($node, 'language')) {
      $lang = $node->language;
    }
    else {
      $lang = 'und';
    }
    return $lang;
  }
}

function unkle_util_get_families_as_spouse($person) {
  $lang = unkle_util_get_language($person);
  if (property_exists($person, 'person_spousetofamily') &&
      is_array($person->person_spousetofamily) &&
      array_key_exists($lang, $person->person_spousetofamily) &&
      count($person->person_spousetofamily[$lang])) {
        $families = array();
        foreach ($person->person_spousetofamily[$lang] as $fam) {
          $families[] = $fam['target_id'];
        }
        return $families;
  }
  else  {
    return NULL;
  }
}

function unkle_util_get_spouse_for_family($person, $family) {
  $husband = unkle_util_get_family_husband_nid($family);
  $wife = unkle_util_get_family_wife_nid($family);
  if ($husband == $person->nid && $wife) {
    return $wife;
  }
  if ($wife == $person->nid && $husband) {
    return $husband;
  }
  return NULL;
}

function unkle_util_get_spouses($person) {
  $families = unkle_util_get_families_as_spouse($person);
  if ($families) {
    $spouses = array();
    foreach ($families as $familyid) {
      $family = node_load($familyid);
      $husband = unkle_util_get_family_husband_nid($family);
      $wife = unkle_util_get_family_wife_nid($family);
      if ($husband = $person->nid && $wife) {
        $spouses[] = $wife;
      }
      if ($wife = $person->nid && $husband) {
        $spouses[] = $husband;
      }
    }
    return $spouses;
  }  
  return NULL;
}

function unkle_util_get_half_siblings($person) {
  $lang = unkle_util_get_language($person);
  if ($family = node_load(unkle_util_get_person_childtofamily_nid($person))) {
    $father = node_load(unkle_util_get_family_husband_nid($family));
    $mother = node_load(unkle_util_get_family_wife_nid($family));
    $parent_families = array();
    if ($father_families = unkle_util_get_families_as_spouse($father)) {
      $parent_families = $father_families;
    }
    if ($mother_families = unkle_util_get_families_as_spouse($mother)) {
      $parent_families = array_merge($parent_families, $mother_families);
    }

    $other_families = array();
    foreach ($parent_families as $next_family) {
      if ($next_family != $family->nid) {
        $other_families[] = $next_family;
      }
    }
    $other_families = array_unique($other_families);
    $half_siblings = array();
    if (count($other_families)) {      
      foreach ($other_families as $next_family) {
        if ($children = unkle_util_get_family_children(node_load($next_family))) {
          $half_siblings = array_merge($half_siblings, $children);
        }
      }
      return $half_siblings;
    }
  }
}

function unkle_util_get_spouse_title($spouse_id) {
  $title = '';
  $person = node_load($spouse_id);
  $lang = unkle_util_get_language($person);
  if (property_exists($person, 'person_surname') &&
      is_array($person->person_surname) &&
      count($person->person_surname) &&
      $person->person_surname[$lang][0]['value']) {
    $title .= $person->person_surname[$lang][0]['value'];
  }
  else  {
    $title .= 'Unknown';
  }
  if (property_exists($person, 'person_birthdate') &&
      is_array($person->person_birthdate) &&
      count($person->person_birthdate) &&
      $person->person_birthdate[$lang][0]['value']) {
    $dateparts = date_parse($person->person_birthdate[$lang][0]['value']);
    if ($dateparts['year']) {
      $title .= ' [b:' . $dateparts['year'] . ']';
    }
  }
  return $title;
}

function unkle_util_show_spouses_and_children($person) {
  if ($families = unkle_util_get_families_as_spouse($person)) {
    $out = '<div class="field unkle-spouse-and-children">';
    $out .= '<div class="field-label">' . t("Spouse(s) and Children:") . '</div>';
    foreach ($families as $familyid) {
      $family = node_load($familyid);
      $spouse = unkle_util_get_spouse_for_family($person, $family);
      $children = unkle_util_get_family_children($family);
      $out .= unkle_util_get_person_title_nodelink(node_load($spouse)) . '<br />';
      if ($children = unkle_util_get_family_children($family)) {
        foreach ($children as $child) {
          $out .= ' - ' . unkle_util_get_person_title_nodelink(node_load($child)) . '<br />';
        }
      }
    }
    $out .= '</div></div>';
    return $out;
  }
  return NULL;
}

