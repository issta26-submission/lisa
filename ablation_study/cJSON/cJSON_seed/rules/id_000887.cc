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
//<ID> 887
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
    cJSON *num_a = NULL;
    cJSON *num_b = NULL;
    cJSON *num_c = NULL;
    int size_before = 0;
    int size_after = 0;
    double first_val = 0.0;
    double pi_val = 0.0;
    char buffer[32];
    memset(buffer, 0, sizeof(buffer));

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "numbers", arr);
    num_a = cJSON_CreateNumber(10.0);
    cJSON_AddItemToArray(arr, num_a);
    num_b = cJSON_CreateNumber(20.5);
    cJSON_AddItemToArray(arr, num_b);
    num_c = cJSON_CreateNumber(-3.25);
    cJSON_AddItemToArray(arr, num_c);
    cJSON_AddNumberToObject(root, "pi", 3.1415);

    // step 3: Operate / Validate
    size_before = cJSON_GetArraySize(arr);
    first_val = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 0));
    pi_val = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "pi"));
    cJSON_DeleteItemFromArray(arr, 1);
    size_after = cJSON_GetArraySize(arr);
    buffer[0] = (char)('0' + (size_before >= 0 && size_before < 10 ? size_before : 9));
    buffer[1] = (char)('0' + (size_after >= 0 && size_after < 10 ? size_after : 9));
    (void)first_val;
    (void)pi_val;

    // step 4: Cleanup
    cJSON_Delete(root);
    memset(buffer, 0, sizeof(buffer));

    // API sequence test completed successfully
    return 66;
}