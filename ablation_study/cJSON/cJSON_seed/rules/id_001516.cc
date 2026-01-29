#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 1516
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *parsed = NULL;
    cJSON *nameItem = NULL;
    cJSON *ref = NULL;
    cJSON *nameCopy = NULL;
    char *nameVal = NULL;
    char *refVal = NULL;
    const char *parse_end = NULL;
    char json[] = " { \"name\" : \"Alice\", \"details\" : { \"nickname\" : \"Al\" } } ";

    // step 2: Setup / Configure
    parsed = cJSON_ParseWithOpts(json, &parse_end, 1);
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "imported", parsed);
    ref = cJSON_CreateStringReference("Bob");
    cJSON_AddItemToObject(root, "name_ref", ref);

    // step 3: Operate / Validate
    nameItem = cJSON_GetObjectItem(parsed, "name");
    nameVal = cJSON_GetStringValue(nameItem);
    nameCopy = cJSON_CreateStringReference(nameVal);
    cJSON_AddItemToObject(root, "name_copy", nameCopy);
    refVal = cJSON_GetStringValue(cJSON_GetObjectItem(root, "name_ref"));
    (void)refVal;
    (void)parse_end;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}