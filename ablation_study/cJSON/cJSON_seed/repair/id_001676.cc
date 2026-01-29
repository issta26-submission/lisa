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
//<ID> 1676
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_AddObjectToObject(root, "child");
    cJSON *greet = cJSON_CreateString("hello");
    cJSON_AddItemToObject(child, "greet", greet);
    cJSON *num = cJSON_AddNumberToObject(child, "value", 3.1415);
    cJSON *f = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "is_false", f);

    // step 2: Configure
    cJSON *ref_obj = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "ref", ref_obj);
    cJSON *ref_num = cJSON_AddNumberToObject(ref_obj, "ref_num", 7.0);

    // step 3: Operate and Validate
    cJSON_bool false_flag = cJSON_IsFalse(f);
    char *printed = cJSON_PrintBuffered(root, 256, 1);
    cJSON *greet_item = cJSON_GetObjectItem(child, "greet");
    char *greet_val = cJSON_GetStringValue(greet_item);
    double num_val = cJSON_GetNumberValue(num);
    cJSON *ref_num_item = cJSON_GetObjectItem(ref_obj, "ref_num");
    double ref_val = cJSON_GetNumberValue(ref_num_item);
    char buffer[128];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = (char)('0' + (int)(false_flag ? 1 : 0));
    buffer[2] = greet_val ? greet_val[0] : 'Y';
    buffer[3] = (char)('0' + (int)num_val % 10);
    buffer[4] = (char)('0' + (int)ref_val % 10);
    (void)buffer;
    (void)greet_item;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}