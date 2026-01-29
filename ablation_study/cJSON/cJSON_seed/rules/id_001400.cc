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
//<ID> 1400
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *num_item = NULL;
    cJSON *str_item = NULL;
    char *buffer = NULL;
    const int buf_len = 256;
    cJSON_bool printed_ok = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num_item = cJSON_CreateNumber(42.0);
    str_item = cJSON_CreateString("hello");
    cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddStringToObject(root, "label", "demo");
    cJSON_AddItemReferenceToObject(root, "items_ref", arr);
    buffer = (char *)cJSON_malloc((size_t)buf_len);

    // step 3: Operate / Validate
    printed_ok = cJSON_PrintPreallocated(root, buffer, buf_len, 1);
    (void)printed_ok;
    (void)buffer;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}