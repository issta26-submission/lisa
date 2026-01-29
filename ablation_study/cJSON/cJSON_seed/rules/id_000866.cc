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
//<ID> 866
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
    cJSON *str_ref = NULL;
    cJSON *num_created = NULL;
    cJSON *num_added = NULL;
    cJSON *got_item0 = NULL;
    cJSON *got_item1 = NULL;
    char *printed = NULL;
    char buffer[128];
    memset(buffer, 0, sizeof(buffer));
    int arr_size = 0;
    const char *got_str = NULL;
    double got_num = 0.0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_AddArrayToObject(root, "items");
    str_ref = cJSON_CreateStringReference("hello_ref");
    cJSON_InsertItemInArray(arr, 0, str_ref);
    num_created = cJSON_CreateNumber(99.0);
    cJSON_AddItemToArray(arr, num_created);
    num_added = cJSON_AddNumberToObject(root, "pi", 3.1415);

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    got_item0 = cJSON_GetArrayItem(arr, 0);
    got_item1 = cJSON_GetArrayItem(arr, 1);
    got_str = cJSON_GetStringValue(got_item0);
    got_num = cJSON_GetNumberValue(got_item1);
    printed = cJSON_PrintUnformatted(root);
    buffer[0] = got_str ? got_str[0] : '\0';
    (void)arr_size;
    (void)got_num;
    (void)num_added;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    memset(buffer, 0, sizeof(buffer));

    // API sequence test completed successfully
    return 66;
}