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
//<ID> 886
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
    double first_val = 0.0;
    double second_val = 0.0;
    int initial_size = 0;
    int size_after_delete = 0;
    char buffer[128];
    memset(buffer, 0, sizeof(buffer));

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "numbers", arr);
    n1 = cJSON_CreateNumber(10.0);
    cJSON_AddItemToArray(arr, n1);
    n2 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(arr, n2);
    n3 = cJSON_CreateNumber(30.0);
    cJSON_AddItemToArray(arr, n3);
    cJSON_AddNumberToObject(root, "pi", 3.1415);

    // step 3: Operate / Validate
    initial_size = cJSON_GetArraySize(arr);
    first_val = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 0));
    second_val = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 1));
    cJSON_DeleteItemFromArray(arr, 1);
    size_after_delete = cJSON_GetArraySize(arr);
    printed = cJSON_PrintUnformatted(root);
    buffer[0] = printed ? printed[0] : '\0';
    (void)first_val;
    (void)second_val;
    (void)initial_size;
    (void)size_after_delete;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    memset(buffer, 0, sizeof(buffer));

    // API sequence test completed successfully
    return 66;
}