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
//<ID> 202
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num = cJSON_CreateNumber(1.0);
    cJSON *child = cJSON_CreateObject();
    cJSON *str = cJSON_CreateString("example");
    cJSON *false_item = NULL;
    void *buffer = cJSON_malloc(128);
    memset(buffer, 0, 128);
    char *printed = NULL;
    double adjusted_number = 0.0;
    const char *extracted_text = NULL;
    cJSON_bool is_false = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "num", num);
    cJSON_AddItemToObject(root, "nested", child);
    cJSON_AddItemToObject(child, "text", str);
    false_item = cJSON_AddFalseToObject(root, "enabled");
    adjusted_number = cJSON_SetNumberHelper(num, 3.14159);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    ((char *)buffer)[0] = printed[0];
    extracted_text = cJSON_GetStringValue(cJSON_GetObjectItem(child, "text"));
    is_false = cJSON_IsFalse(false_item);
    (void)adjusted_number;
    (void)extracted_text;
    (void)is_false;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    cJSON_free(buffer);

    // API sequence test completed successfully
    return 66;
}