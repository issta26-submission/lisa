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
//<ID> 2193
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    int initial_numbers[3] = {10, 20, 30};
    cJSON *root = cJSON_CreateObject();
    cJSON *numbers = cJSON_CreateIntArray(initial_numbers, 3);
    cJSON_AddItemToObject(root, "numbers", numbers);

    // step 2: Configure
    cJSON *insert_num = cJSON_CreateNumber(99.0);
    cJSON_InsertItemInArray(numbers, 1, insert_num);
    cJSON *detached_item = cJSON_DetachItemFromArray(numbers, 2);
    cJSON *adjusted = cJSON_CreateNumber(cJSON_GetNumberValue(detached_item) + 5.0);
    cJSON_InsertItemInArray(numbers, 3, adjusted);

    // step 3: Operate and Validate
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = (json && json[1]) ? json[1] : '\0';
    buffer[2] = (json && json[2]) ? json[2] : '\0';
    buffer[3] = '\0';
    cJSON_free(buffer);
    cJSON_free(json);

    // step 4: Cleanup
    cJSON_Delete(detached_item);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}