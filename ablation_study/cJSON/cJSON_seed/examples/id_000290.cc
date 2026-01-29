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
//<ID> 290
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON *name = cJSON_CreateString("example_name");
    cJSON *answer = cJSON_CreateNumber(42.0);
    cJSON *flag = cJSON_CreateTrue();

    // step 2: Configure
    cJSON_AddItemToObjectCS(root, "meta", meta);
    cJSON_AddItemToObjectCS(root, "answer", answer);
    cJSON_AddItemToObject(meta, "name", name);
    cJSON_AddItemToObjectCS(meta, "flag", flag);

    // step 3: Operate & Validate
    cJSON_bool has_meta = cJSON_HasObjectItem(root, "meta");
    cJSON *has_meta_flag = cJSON_CreateBool(has_meta);
    cJSON_AddItemToObjectCS(root, "has_meta", has_meta_flag);
    cJSON *got_meta = cJSON_GetObjectItem(root, "meta");
    cJSON *got_name = cJSON_GetObjectItem(got_meta, "name");
    char *name_text = cJSON_GetStringValue(got_name);
    cJSON_AddItemToObjectCS(root, "name_copy", cJSON_CreateString(name_text));
    char *printed = cJSON_Print(root);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "answer");
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}