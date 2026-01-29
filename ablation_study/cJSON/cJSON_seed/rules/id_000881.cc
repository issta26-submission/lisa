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
//<ID> 881
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
    cJSON *n1 = NULL;
    cJSON *n2 = NULL;
    cJSON *n3 = NULL;
    char *printed = NULL;
    int size_before = 0;
    int size_after = 0;
    double mid_val = 0.0;
    double new_mid_val = 0.0;
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "numbers", arr);
    n1 = cJSON_CreateNumber(10.5);
    cJSON_AddItemToArray(arr, n1);
    n2 = cJSON_CreateNumber(20.75);
    cJSON_AddItemToArray(arr, n2);
    n3 = cJSON_CreateNumber(30.125);
    cJSON_AddItemToArray(arr, n3);
    cJSON_AddNumberToObject(root, "extra_value", 99.99);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    size_before = cJSON_GetArraySize(arr);
    mid_val = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 1));
    cJSON_DeleteItemFromArray(arr, 1);
    size_after = cJSON_GetArraySize(arr);
    new_mid_val = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 1));
    buffer[0] = (char)('0' + (size_before > 9 ? 9 : size_before));
    buffer[1] = ',';
    buffer[2] = (char)('0' + (size_after > 9 ? 9 : size_after));
    buffer[3] = ',';
    buffer[4] = (char)('0' + (int)(mid_val));
    buffer[5] = ',';
    buffer[6] = (char)('0' + (int)(new_mid_val));
    (void)buffer;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    memset(buffer, 0, sizeof(buffer));

    // API sequence test completed successfully
    return 66;
}