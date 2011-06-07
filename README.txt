
 islandora_oai based on oai2ForCCK.module


Description:
============
This module provides an implementation the Open Archives Initiative Protocol for Metadata Harvesting (OAI-PMH)
for Islandora fedora repository with a solr index. By implementing the islanodra_oa module,
you can expose content (its metadata) as an OAI-PMH repository. It will then be accessible by OAI harvesters.
For further OAI documentation, please see here: http://www.openarchives.org/OAI/openarchivesprotocol.html

Functionality:
=============
- Receive OAI-PMH request of service providers in form of HTTP request (GET or POST)
- Handle the OAI-PMH request
- Respond to OAI-PMH request in form of HTTP response (XML) to the service provider


Requirements:
=============
- Drupal 6.x
- fedora repository
- islandora solr client


Currently looks for a MODS stream and uses the LOC MODS to OAI_DC xlst to transform to oai_dc.  If there is not a MODS stream it will use Fedora's DC.

Has limited support for sets.  A set is defined by a fedora Object thats cmodel object has a cmodel of islandora:collectionCModel.  For example if object1 is related to object2 with a relationship
of isMemberOf or isMemberOfCollection and objects2 has a contentModel whose contentModel is islandora:collectionCmodel your good.

ListSets does an ITQL query to get a list of collection objects:
select $object $title $content from <#ri>
                             where ($object <info:fedora/fedora-system:def/model#label> $title
                             and $object <fedora-model:hasModel> $content
                             and ($content <fedora-model:hasModel> <info:fedora/islandora:collectionCModel>)
                             and $object <fedora-model:state> <info:fedora/fedora-system:def/model#Active>)
                             order by $title

Your solr index should have a field PID that is the unique identifier and a timestamp stored in fgs.lastModifiedDate (the date field is subject to change and both of these could probably be defined in the configuration)

Installation:
=============

   1. Copy the module directory to your modules directory.
   2. Browse to admin/modules and enable the islandora_oai module, the module will report success or failure of the installation.
   3. Browse to admin/settings/islandora_oai to set the basic configuration of your repository
     
   4. After you have exposed content types and some fields, your repository is available at /oai2
   5. example requests are as follows:
      - /oai2?verb=Identify
      - /oai2?verb=ListMetadataFormats (only oai_dc currently supported)
      - /oai2?verb=ListIdentifiers&metadataPrefix=oai_dc
      - /oai2?verb=GetRecord&metadataPrefix=oai_dc&identifier=PID
      - /oai2?verb=ListRecords&metadataPrefix=oai_dc
     
      
      

Other Projects:
====================
As an alternative, check out the OAI-PHM module (http://drupal.org/project/oai2). It works hand in hand with the
bibliography module to expose biblio content in an OAI-PHM repository.
