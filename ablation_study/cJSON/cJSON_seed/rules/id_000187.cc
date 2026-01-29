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
//<ID> 187
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON *label = cJSON_CreateString("answer");
    const char *version_str = cJSON_Version();
    cJSON *version_ref = cJSON_CreateStringReference(version_str);
    void *buffer = cJSON_malloc(128);
    memset(buffer, 0, 128);
    cJSON *retrieved_num = NULL;
    cJSON *retrieved_label = NULL;
    double numeric_value = 0.0;
    cJSON_bool label_is_string = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "number", num);
    cJSON_AddItemToObject(root, "label", label);
    cJSON_AddItemToObject(root, "version", version_ref);

    // step 3: Operate / Validate
    retrieved_num = cJSON_GetObjectItem(root, "number");
    numeric_value = cJSON_GetNumberValue(retrieved_num);
    retrieved_label = cJSON_GetObjectItem(root, "label");
    label_is_string = cJSON_IsString(retrieved_label);
    (void)numeric_value;
    (void)label_is_string;
    (void)version_str;
    (void)buffer;

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_free(buffer);

    // API sequence test completed successfully
    return 66;
}