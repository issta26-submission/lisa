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
//<ID> 554
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    cJSON *root = cJSON_CreateObject();
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *str_ref = cJSON_CreateStringReference("example");
    cJSON *num_item = NULL;
    char *retrieved_str = NULL;
    double num_value = 0.0;
    char first_char = '\0';

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "flag", true_item);
    num_item = cJSON_AddNumberToObject(root, "count", 42.0);
    cJSON_AddItemToObject(root, "label", str_ref);

    // step 3: Operate / Validate
    retrieved_str = cJSON_GetStringValue(str_ref);
    first_char = retrieved_str[0];
    num_value = cJSON_GetNumberValue(num_item);
    buffer[0] = first_char;
    buffer[1] = (char)((int)num_value & 0x7F);
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}