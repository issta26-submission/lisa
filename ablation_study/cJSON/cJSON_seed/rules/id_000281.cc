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
//<ID> 281
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(123.456);
    cJSON *s1 = cJSON_CreateString("example");
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    int arr_size = 0;
    cJSON *got_arr = NULL;
    cJSON *got_extra = NULL;
    double extra_val = 0.0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "data", arr);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddNumberToObject(root, "extra_number", 7.5);
    cJSON_AddStringToObject(root, "note", "added");

    // step 3: Operate / Validate
    got_arr = cJSON_GetObjectItem(root, "data");
    arr_size = cJSON_GetArraySize(got_arr);
    got_extra = cJSON_GetObjectItem(root, "extra_number");
    extra_val = cJSON_GetNumberValue(got_extra);
    buffer[0] = (char)('0' + (arr_size % 10));
    buffer[1] = (char)('A' + (((int)extra_val) % 26));
    (void)buffer[0];
    (void)buffer[1];

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}