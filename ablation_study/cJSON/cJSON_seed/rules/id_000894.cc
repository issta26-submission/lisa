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
//<ID> 894
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
    cJSON *num0 = NULL;
    cJSON *num1 = NULL;
    cJSON *num_repl = NULL;
    cJSON *retrieved_items = NULL;
    cJSON *retrieved_scalar = NULL;
    char *print_before = NULL;
    char *print_after = NULL;
    char *buffer = NULL;
    cJSON_bool is_number = 0;
    cJSON_bool replaced = 0;
    double scalar_val = 0.0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    num0 = cJSON_CreateNumber(10.5);
    cJSON_AddItemToArray(arr, num0);
    num1 = cJSON_CreateNumber(20.75);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddNumberToObject(root, "scalar", 3.14);
    buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    buffer[0] = 'A';
    print_before = cJSON_Print(root);

    // step 3: Operate / Validate
    retrieved_items = cJSON_GetObjectItem(root, "items");
    retrieved_scalar = cJSON_GetObjectItem(root, "scalar");
    is_number = cJSON_IsNumber(retrieved_scalar);
    num_repl = cJSON_CreateNumber(99.99);
    replaced = cJSON_ReplaceItemInArray(retrieved_items, 1, num_repl);
    print_after = cJSON_Print(root);
    buffer[1] = (char)(is_number + replaced + 65);
    (void)scalar_val;
    (void)num0;
    (void)num1;

    // step 4: Cleanup
    cJSON_free(print_before);
    cJSON_free(print_after);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}