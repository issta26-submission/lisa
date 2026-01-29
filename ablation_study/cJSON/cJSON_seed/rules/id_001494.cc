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
//<ID> 1494
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
    cJSON *arr = NULL;
    cJSON *flag_in_parsed = NULL;
    cJSON *embedded = NULL;
    cJSON *list_from_root = NULL;
    const char json[] = " { \"numbers\": [1, 2], \"flag\": true } ";
    char prebuf[128];
    memset(prebuf, 0, sizeof(prebuf));
    cJSON_bool flag_is_true = 0;
    int list_size = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", arr);
    parsed = cJSON_Parse(json);
    cJSON_AddItemToObject(root, "parsed", parsed);
    cJSON_AddBoolToObject(root, "root_flag", 1);

    // step 3: Operate / Validate
    flag_in_parsed = cJSON_GetObjectItem(parsed, "flag");
    flag_is_true = cJSON_IsTrue(flag_in_parsed);
    embedded = cJSON_GetObjectItem(root, "parsed");
    list_from_root = cJSON_GetObjectItem(root, "list");
    list_size = cJSON_GetArraySize(list_from_root);
    (void)flag_is_true;
    (void)embedded;
    (void)list_size;
    (void)prebuf;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}