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
//<ID> 1409
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
    cJSON *str_item = NULL;
    cJSON *num_item = NULL;
    char print_buffer[256];
    cJSON_bool printed_ok = 0;
    int array_size = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    str_item = cJSON_CreateString("referenced_string");
    cJSON_AddStringToObject(root, "label", "main_label");
    cJSON_AddItemReferenceToObject(root, "alias", str_item);
    num_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToArray(arr, cJSON_CreateString("element_two"));

    // step 3: Operate / Validate
    memset(print_buffer, 0, sizeof(print_buffer));
    printed_ok = cJSON_PrintPreallocated(root, print_buffer, (int)sizeof(print_buffer), 1);
    array_size = cJSON_GetArraySize(arr);
    (void)printed_ok;
    (void)array_size;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}