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
//<ID> 1647
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *nested = cJSON_AddObjectToObject(root, "nested");
    cJSON *s1 = cJSON_CreateString("value1");
    cJSON_AddItemToObject(nested, "s1", s1);
    cJSON_AddStringToObject(root, "root_msg", "hello_root");

    // step 2: Configure
    cJSON *retrieved = cJSON_GetObjectItem(root, "root_msg");
    const char *retrieved_str = cJSON_GetStringValue(retrieved);
    cJSON *copied = cJSON_CreateString(retrieved_str);
    cJSON_AddItemToObject(nested, "copied_from_root", copied);

    // step 3: Operate & Validate
    char *pretty = cJSON_Print(root);
    cJSON_free(pretty);
    char *compact = cJSON_PrintUnformatted(root);
    cJSON_free(compact);
    cJSON *check = cJSON_GetObjectItemCaseSensitive(nested, "copied_from_root");
    const char *check_str = cJSON_GetStringValue(check);
    cJSON *verification = cJSON_CreateString(check_str);
    cJSON_AddItemToObject(root, "verification", verification);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}