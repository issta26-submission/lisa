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
//<ID> 325
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *parsed = cJSON_ParseWithLength("{\"value\":123.5,\"extra\":7}", 25);
    cJSON *created_num = cJSON_CreateNumber(42.0);
    cJSON *added_num = NULL;
    cJSON_bool add_item_ok = 0;
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    cJSON *parsed_value_item = NULL;
    cJSON *retrieved_newnum = NULL;
    double parsed_value = 0.0;
    double retrieved_value = 0.0;

    // step 2: Setup / Configure
    added_num = cJSON_AddNumberToObject(root, "newnum", 9.0);
    add_item_ok = cJSON_AddItemToObject(root, "copied_number", created_num);

    // step 3: Operate / Validate
    parsed_value_item = cJSON_GetObjectItem(parsed, "value");
    parsed_value = cJSON_GetNumberValue(parsed_value_item);
    retrieved_newnum = cJSON_GetObjectItem(root, "newnum");
    retrieved_value = cJSON_GetNumberValue(retrieved_newnum);
    buffer[0] = (char)('A' + ((int)parsed_value % 26));
    buffer[1] = (char)('A' + ((int)retrieved_value % 26));
    buffer[2] = (char)('0' + (int)add_item_ok);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}