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
//<ID> 380
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *arr = (cJSON *)0;
    cJSON *raw_item = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool raw_flag = 0;
    int prebuffer = 0;
    cJSON_bool fmt = 0;

    // step 2: Initialize
    arr = cJSON_CreateArray();
    raw_item = cJSON_CreateRaw("{\"embedded\":true}");
    num_item = cJSON_CreateNumber(7.5);

    // step 3: Configure
    cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToArray(arr, raw_item);

    // step 4: Operate
    raw_flag = cJSON_IsRaw(raw_item);
    prebuffer = 64 + (int)raw_flag;
    fmt = raw_flag;
    printed = cJSON_PrintBuffered((const cJSON *)arr, prebuffer, fmt);

    // step 5: Validate
    (void)printed;
    (void)raw_flag;
    (void)prebuffer;
    (void)fmt;

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(arr);

    // API sequence test completed successfully
    return 66;
}