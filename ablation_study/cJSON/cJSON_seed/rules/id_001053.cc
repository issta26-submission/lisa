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
//<ID> 1053
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *inner1 = NULL;
    cJSON *inner2 = NULL;
    cJSON *flag = NULL;
    cJSON *ref_to_inner2 = NULL;
    cJSON *replacement = NULL;
    cJSON *ver_item = NULL;
    cJSON *got_one = NULL;
    char *printed = NULL;
    const char *ver = NULL;
    cJSON_bool replaced = 0;
    cJSON_bool was_true = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    inner1 = cJSON_CreateObject();
    inner2 = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "one", inner1);
    cJSON_AddItemToObject(root, "two", inner2);
    flag = cJSON_CreateBool(0);
    cJSON_AddItemToObject(inner1, "enabled", flag);
    cJSON_AddStringToObject(inner2, "name", "secondary");

    // step 3: Operate / Validate
    // Use AddItemReferenceToObject to add a reference to inner2 under "two_ref"
    cJSON_AddItemReferenceToObject(root, "two_ref", inner2);
    replacement = cJSON_CreateBool(1);
    replaced = cJSON_ReplaceItemViaPointer(root, inner1, replacement);
    ver = cJSON_Version();
    ver_item = cJSON_CreateString(ver);
    cJSON_AddItemToObject(root, "version", ver_item);
    got_one = cJSON_GetObjectItem(root, "one");
    was_true = cJSON_IsTrue(got_one);

    // step 4: Cleanup
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66;
}