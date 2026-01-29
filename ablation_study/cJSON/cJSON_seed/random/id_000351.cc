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
//<ID> 351
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *num = (cJSON *)0;
    cJSON *str = (cJSON *)0;
    cJSON *dup = (cJSON *)0;
    cJSON *tmp = (cJSON *)0;
    char *printed = (char *)0;
    char *dup_printed = (char *)0;
    const char *label_str = (const char *)0;
    double answer = 0.0;
    cJSON_bool is_arr = (cJSON_bool)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num = cJSON_CreateNumber(42.0);
    str = cJSON_CreateString("example");

    // step 3: Configure
    cJSON_AddItemToObject(root, "list", arr);
    cJSON_AddItemToObject(root, "answer", num);
    cJSON_AddItemToObject(root, "label", str);
    cJSON_AddRawToObject(root, "raw", "{\"inner\":true,\"n\":7}");

    // step 4: Operate
    is_arr = cJSON_IsArray(arr);
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate
    tmp = cJSON_GetObjectItem(root, "answer");
    answer = cJSON_GetNumberValue(tmp);
    tmp = cJSON_GetObjectItem(root, "label");
    label_str = cJSON_GetStringValue(tmp);
    dup = cJSON_Duplicate(root, (cJSON_bool)1);
    dup_printed = cJSON_PrintUnformatted(dup);
    (void)is_arr;
    (void)answer;
    (void)label_str;

    // step 6: Cleanup
    if (printed) cJSON_free(printed);
    if (dup_printed) cJSON_free(dup_printed);
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}