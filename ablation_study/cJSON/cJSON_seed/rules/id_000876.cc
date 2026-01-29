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
//<ID> 876
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *num_item1 = NULL;
    cJSON *num_item2 = NULL;
    cJSON *retrieved = NULL;
    cJSON *after_del = NULL;
    char *mem_block = NULL;
    char buffer[32];
    double retrieved_value = 0.0;
    cJSON_bool added1 = 0;
    cJSON_bool added2 = 0;
    memset(buffer, 0, sizeof(buffer));

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    num_item1 = cJSON_CreateNumber(123.0);
    num_item2 = cJSON_CreateNumber(456.789);
    added1 = cJSON_AddItemToObject(root, "num1", num_item1);
    added2 = cJSON_AddItemToObject(root, "num2", num_item2);
    mem_block = (char *)cJSON_malloc(16);
    memset(mem_block, 0, 16);
    mem_block[0] = 'A';
    mem_block[1] = '\0';

    // step 3: Operate / Validate
    retrieved = cJSON_GetObjectItem(root, "num2");
    retrieved_value = cJSON_GetNumberValue(retrieved);
    buffer[0] = (added1 ? '1' : '0');
    buffer[1] = (added2 ? '1' : '0');
    buffer[2] = (retrieved ? '1' : '0');
    cJSON_DeleteItemFromObjectCaseSensitive(root, "num1");
    after_del = cJSON_GetObjectItem(root, "num1");
    buffer[3] = (after_del ? '1' : '0');
    (void)retrieved_value;
    (void)mem_block;

    // step 4: Cleanup
    cJSON_free(mem_block);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}