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
//<ID> 492
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
    cJSON *num_item = (cJSON *)0;
    cJSON *added_num = (cJSON *)0;
    char *buffer = (char *)0;
    const int buf_len = 256;
    cJSON_bool printed_ok = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);

    // step 3: Configure
    num_item = cJSON_CreateNumber(1.23);
    cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(4.56));
    cJSON_AddItemToObject(root, "numbers", arr);
    added_num = cJSON_AddNumberToObject(root, "constant", 3.14159);

    // step 4: Operate
    printed_ok = cJSON_PrintPreallocated(root, buffer, buf_len, 1);

    // step 5: Validate
    (void)printed_ok;
    (void)added_num;
    (void)num_item;
    (void)buffer;

    // step 6: Cleanup
    cJSON_free((void *)buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}