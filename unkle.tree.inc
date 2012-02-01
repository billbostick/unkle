<?php
/**
 * @file
 * tree drawing function for the unkle module
 */

/**
 * Implements hook_menu().
 */
function unkle_tree_menu() {
  // tree display menu items
  $items['node/%person_node/tree'] = array(
    'title' => 'Tree',
    'page callback' => 'gedcom_person_tree_horiz',
    'page arguments' => array(1),
    'access callback' => TRUE,
    'type' => MENU_LOCAL_TASK,
    'file' => 'unkle.tree.inc',
  );
  $items['node/%person_node/tree/horiz'] = array(
    'title' => 'Horizontal',
    'page callback' => 'gedcom_person_tree_horiz',
    'page arguments' => array(1),
    'access callback' => TRUE,
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'file' => 'unkle.tree.inc',
  );
  $items['node/%person_node/tree/vert'] = array(
    'title' => 'Vertical',
    'page callback' => 'gedcom_person_tree_vert',
    'page arguments' => array(1),
    'access callback' => TRUE,
    'type' => MENU_LOCAL_TASK,
    'file' => 'unkle.tree.inc',
  );
  return $items;
}

// utility functions
function gedcom_get_pedigree($person, $levels = 3) {
  include_once("unkle.util.inc");
  // limit level count
  if ($levels > 6) {
    $levels = 6;
  }
  $treearray[0][1]['title'] = $person->title;
  $treearray[0][1]['name'] = unkle_util_get_person_fullname($person);
  $treearray[0][1]['nid'] = $person->nid;
  for ($level = 1; $level <= $levels; $level++) {
    for ($familyno = 1; $familyno <= pow(2, $level); $familyno++) {
      $fam = NULL;
      if (array_key_exists('nid', $treearray[($level-1)][(ceil($familyno/2))])) {
        $childid = $treearray[($level-1)][(ceil($familyno/2))]['nid'];      
      }
      else {
        $childid = NULL;
      }
      if ($childid) {
        $child = node_load($childid);
        if ($child->person_childtofamily) {
          $famid = unkle_util_get_person_childtofamily_nid($child);
          $fam = node_load($famid);
        }
      }
      if ($fam) {
        if (array_key_exists('und', $fam->family_husband)) {
          $fatherid = unkle_util_get_family_husband_nid($fam);
          $father = node_load($fatherid);
          $treearray[$level][$familyno]['nid'] = $fatherid;
          $treearray[$level][$familyno]['title'] = $father->title;
          $treearray[$level][$familyno]['name'] = unkle_util_get_person_fullname($father);
        }
        else {
          $treearray[$level][$familyno]['nid'] = 0;
          $treearray[$level][$familyno]['name'] = 'Unknown';
          $treearray[$level][$familyno]['title'] = 'Unknown';
        }
        $familyno++;;
        if (array_key_exists('und', $fam->family_wife)) {
          $motherid = unkle_util_get_family_wife_nid($fam);
          $mother = node_load($motherid);
          $treearray[$level][$familyno]['nid'] = $motherid;
          $treearray[$level][$familyno]['title'] = $mother->title;
          $treearray[$level][$familyno]['name'] = unkle_util_get_person_fullname($mother);
        }
        else {
          $treearray[$level][$familyno]['nid'] = 0;
          $treearray[$level][$familyno]['name'] = 'Unknown';
          $treearray[$level][$familyno]['title'] = 'Unknown';
        }
      }
      else {
        $treearray[$level][$familyno]['empty'] = TRUE;
      }
    }
  }
  return $treearray;
}

function gedcom_get_pedigree_flat($person, $levels = 3) {
  $treearray = gedcom_get_pedigree($person, 3); 
  $tree_people = array();
  $tree_person = array();
  foreach ($treearray as $gen) {
    foreach ($gen as $gen_person) {
      if (array_key_exists('name', $gen_person)) {
        $tree_person['title'] = $gen_person['title'];
        $tree_person['name'] = $gen_person['name'];
        $tree_person['nid'] = $gen_person['nid'];
        $tree_person['link'] = '<a href="/node/' . $tree_person['nid'] . '/tree" ' .
                               'title="' . $tree_person['title'] . '">' .
                               $tree_person['name'] . '</a>';
      }
      else {
        $tree_person['name'] = 'Unknown';
        $tree_person['nid'] = 0;
        $tree_person['link'] = 'Unknown';
      }
      $tree_people[] = $tree_person;
    }
  }  
  return $tree_people;
}

function gedcom_pedigree_table_vert($person, $levels) {
  $treearray = gedcom_get_pedigree($person, $levels);
  $tree = '';
  $cellwidth = 120;
  $toprowcells = pow(2, $levels);
  $totalwidth = $toprowcells * $cellwidth;
  $tree = '<table><tr><td><table width=95%>';
  for ($level = $levels; $level >= 0; $level--) {
    $tree .= '<tr>';
    for ($familyno = 1; $familyno <= pow(2, $level); $familyno++) {
      $tree .= '<td width=' . ($totalwidth/pow(2, $level)) . 'px colspan=' . ($toprowcells/pow(2, $level)) . '><p align=center>';
      if (isset($treearray[$level][$familyno]['name'])) {
        $tree .= '<a href="/node/' . $treearray[$level][$familyno]['nid'] . '/tree" title="' . $treearray[$level][$familyno]['title'] . '">' . $treearray[$level][$familyno]['name'] . '</a>';
      }
      else {
        $tree .= 'Unknown';
      }
      $tree .= '</td>';
    }
    $tree .= '</tr>';
  }
  $tree .= '</table></td></td width=50%></td></tr></table>';
  return $tree;
}

function gedcom_pedigree_table_horiz($person, $levels) {
  $tree_people = gedcom_get_pedigree_flat($person, 3); 
// horizontal table
  // deal with the complexities of HTML tables that use nested "rowspan" tags
  $tree = '<table><tr><td><table border="1">';
    $tree .= '<tr>';
      $tree .= '<td rowspan="8">' . $tree_people[0]['link'] . '</td>';
      $tree .= '<td rowspan="4">' . $tree_people[1]['link'] . '</td>';
      $tree .= '<td rowspan="2">' . $tree_people[3]['link'] . '</td>';
      $tree .= '<td>' . $tree_people[7]['link'] . '</td>';
    $tree .= '</tr>';
                          
    $tree .= '<tr>';
      $tree .= '<td>' . $tree_people[8]['link'] . '</td>';
    $tree .= '</tr>';
                                    
    $tree .= '<tr>';
      $tree .= '<td rowspan="2">' . $tree_people[4]['link'] . '</td>';
      $tree .= '<td>' . $tree_people[9]['link'] . '</td>';
    $tree .= '</tr>';
                                                
    $tree .= '<tr>';
      $tree .= '<td>' . $tree_people[10]['link'] . '</td>';
    $tree .= '</tr>';
                                                        
    $tree .= '<tr>';
      $tree .= '<td rowspan="4">' . $tree_people[2]['link'] . '</td>';
      $tree .= '<td rowspan="2">' . $tree_people[5]['link'] . '</td>';
      $tree .= '<td>' . $tree_people[11]['link'] . '</td>';
    $tree .= '</tr>';
                                                                        
    $tree .= '<tr>';
      $tree .= '<td>' . $tree_people[12]['link'] . '</td>';
    $tree .= '</tr>';
                                                                                
    $tree .= '<tr>';
      $tree .= '<td rowspan="2">' . $tree_people[6]['link'] . '</td>';
      $tree .= '<td>' . $tree_people[13]['link'] . '</td>';
    $tree .= '</tr>';
                                                                                            
    $tree .= '<tr>';
      $tree .= '<td>' . $tree_people[14]['link'] . '</td>';
    $tree .= '</tr>';

  $tree .= '</table></tr></td></table>';

  return $tree;

}

function gedcom_person_tree_horiz($person) {
  $out = gedcom_pedigree_table_horiz($person, 3);
  $out .= gedcom_get_siblings($person);
  return $out;
}

function gedcom_person_tree_vert($person) {
  $out = gedcom_pedigree_table_vert($person, 3);
  $out .= gedcom_get_siblings($person);
  return $out;
}

function gedcom_get_siblings($person) {
  include_once("unkle.util.inc");
  // display spouse(es) and children
  $out = '';
  if (is_array($person->person_spousetofamily) && count($person->person_spousetofamily)) {
    $out .= '<h3>' . t("Spouse(s) and Children") . '</h3>';
    foreach ($person->person_spousetofamily['und'] as $famgroup) {
      $family = node_load($famgroup['nid']);

      if ($family->family_wife['und'][0]['nid'] == $person->nid) {
        $spouse = $family->family_husband;
      }
      else {
        $spouse = $family->family_wife;
      }

      if (is_array($spouse) && count($spouse)) {
        $out .= '<ul>';
        $spousenode = node_load($spouse['und'][0]['nid']);
        $out .= '<li><a href="/node/' . $spousenode->nid . '/tree">' .
                  $spousenode->title . '</a></li>';
      }
      if (is_array($family->family_child) && count($family->family_child)) {
        $out .= '<ul>';
        foreach ($family->family_child['und'] as $childrec) {
          $child = node_load($childrec['nid']);
          $out .= '<li><a href="/node/' . $child->nid . '/tree">' .
                  $child->title . '</a></li>';
        }
        $out .= '</ul>';
      }
      if (is_array($spouse) && count($spouse)) {
        $out .= '</ul>';
      }
    }
  }
  if ($siblings = unkle_util_get_siblings($person)) {
//if ($siblings = unkle_get_siblings($person)) {
    $out .= '<h3>Siblings</h3>';
    $out .= '<ul>';
    foreach ($siblings as $sibling_id) {
      $sib = node_load($sibling_id);
      $out .= '<li><a href="/node/' . $sib->nid . '/tree">' .
              $sib->title . '</a></li>';
    }
    $out .= '</ul>';
  }
 
  return $out;
}



