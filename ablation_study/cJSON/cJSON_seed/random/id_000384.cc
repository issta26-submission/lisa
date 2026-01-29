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
//<ID> 384
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
    cJSON *raw = (cJSON *)0;
    cJSON *got_num = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool is_raw_flag = 0;
    cJSON_bool fmt = 1;
    int prebuffer = 128;
    double extracted = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num = cJSON_CreateNumber(123.0);
    raw = cJSON_CreateRaw("{\"embedded\":true}");

    // step 3: Configure
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToArray(arr, num);
    cJSON_AddItemToArray(arr, raw);

    // step 4: Operate
    is_raw_flag = cJSON_IsRaw(raw);
    printed = cJSON_PrintBuffered(root, prebuffer, fmt);

    // step 5: Validate
    got_num = cJSON_GetArrayItem(arr, 0);
    extracted = cJSON_GetNumberValue(got_num);
    (void)is_raw_flag;
    (void)printed;
    (void)extracted;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}