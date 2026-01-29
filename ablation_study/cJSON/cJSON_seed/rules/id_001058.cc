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
//<ID> 1058
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *primary = NULL;
    cJSON *secondary = NULL;
    cJSON *secondary_dup = NULL;
    cJSON *secondary_ref = NULL;
    cJSON *replacement = NULL;
    char *printed = NULL;
    const char *ver = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    primary = cJSON_CreateObject();
    cJSON_AddStringToObject(primary, "id", "primary_node");
    cJSON_AddNumberToObject(primary, "count", 7.0);
    cJSON_AddItemToObject(root, "primary", primary);
    secondary = cJSON_CreateObject();
    cJSON_AddStringToObject(secondary, "id", "secondary_node");
    cJSON_AddNumberToObject(secondary, "count", 3.0);
    cJSON_AddItemToObject(root, "secondary", secondary);
    secondary_dup = cJSON_Duplicate(secondary, 1);
    secondary_ref = cJSON_CreateObjectReference(secondary_dup);
    cJSON_AddItemToObject(root, "secondary_ref", secondary_ref);
    ver = cJSON_Version();
    cJSON_AddStringToObject(root, "cjson_version", ver);

    // step 3: Operate / Validate
    replacement = cJSON_CreateBool(0);
    cJSON_ReplaceItemViaPointer(root, primary, replacement);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66;
}