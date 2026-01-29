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
//<ID> 195
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num = cJSON_CreateNumber(3.1415);
    cJSON *lbl = cJSON_CreateString("pi");
    cJSON_AddItemToObject(root, "number", num);
    cJSON_AddItemToObject(root, "label", lbl);

    // step 2: Configure
    cJSON *newnum = cJSON_CreateNumber(2.71828);
    cJSON_bool replaced = cJSON_ReplaceItemInObjectCaseSensitive(root, "number", newnum);
    cJSON *num_item = cJSON_GetObjectItemCaseSensitive(root, "number");
    cJSON *label_item = cJSON_GetObjectItemCaseSensitive(root, "label");

    // step 3: Operate and Validate
    double number_value = cJSON_GetNumberValue(num_item);
    cJSON_bool label_is_string = cJSON_IsString(label_item);
    char *unformatted = cJSON_PrintUnformatted(root);
    cJSON_Minify(unformatted);
    void *tmp = cJSON_malloc(64);
    memset(tmp, 0, 64);
    cJSON *parsed = cJSON_Parse(unformatted);
    cJSON_bool equal = cJSON_Compare(root, parsed, 1);
    (void)replaced;
    (void)number_value;
    (void)label_is_string;
    (void)equal;

    // step 4: Cleanup
    cJSON_free(unformatted);
    cJSON_free(tmp);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}