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
//<ID> 1071
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_AddArrayToObject(root, "items");
    cJSON *first = cJSON_CreateString("one");
    cJSON_AddItemToArray(items, first);
    cJSON *second = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(items, second);

    // step 2: Configure
    cJSON *subobj = cJSON_CreateObject();
    cJSON_AddStringToObject(subobj, "info", "sub");
    cJSON_AddItemToArray(items, subobj);
    cJSON *parsed = cJSON_Parse("{\"external\":true}");
    cJSON_AddItemToArray(items, parsed);
    cJSON_AddItemReferenceToObject(root, "first_ref", first);
    cJSON_AddItemReferenceToObject(root, "parsed_ref", parsed);

    // step 3: Operate and Validate
    char *printed = cJSON_Print(root);
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}