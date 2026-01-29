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
//<ID> 1326
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *val = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "value", val);
    cJSON_AddItemToObject(root, "list", arr);

    // step 2: Configure
    cJSON *newnum = cJSON_CreateNumber(1.0);
    cJSON_SetNumberHelper(newnum, 123.456); // adjust numeric value before insertion
    cJSON_AddItemReferenceToArray(arr, val); // add a reference to existing "value" into the array

    // step 3: Operate
    cJSON_InsertItemInArray(arr, 0, newnum); // insert the modified number at the start of the array
    const int buf_len = 512;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, buffer, buf_len, 1);

    // step 4: Validate & Cleanup
    (void)printed_ok;
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}