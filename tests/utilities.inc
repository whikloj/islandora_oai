<?php

/**
 * @file
 * Utility functions for the OAI tests.
 */

class IslandoraOAIWebTestCase extends IslandoraWebTestCase {

  /**
   * Accepts OAI requests with Xpaths and returns results from those queries.
   *
   * @param string $request
   * The parameters of the GET request.
   * @param array|string $xpaths
   * The Xpath query(ies) we'll be making to the XML we GET.
   * @param string $prefix
   * The prefix Xpath queries use to represent the OAI-PMH 2.0 namespace.
   * @return array $results
   * An associative array of queries => results.
   */
  public function islandora_oai_query_request($request, $xpaths, $prefix = 'oai') {
    $xml = simplexml_load_string($this->drupalGetContent('http://localhost/oai2?' . $request));
    $xml->registerXPathNamespace($prefix, 'http://www.openarchives.org/OAI/2.0/');
    $results = array();
    foreach ((array) $xpaths as $xpath) {
      $results[$xpath] = $xml->xpath($xpath);
    }
    return $results;
  }

}
