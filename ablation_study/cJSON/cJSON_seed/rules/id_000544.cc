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
//<ID> 544
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    const char *json = "{\"metrics\":{\"pi\":3.1415},\"status\":\"ok\"}";
    cJSON *root = cJSON_Parse(json);
    cJSON *pi_item = cJSON_CreateNumber(3.14159265);
    char *printed = NULL;
    const double e_value = 2.71828182;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "pi_copy", pi_item);
    cJSON_AddNumberToObject(root, "e", e_value);

    // step 3: Operate / Validate
    printed = cJSON_Print(root);
    size_t printed_len = strlen(printed);
    size_t copy_len = printed_len < 252 ? printed_len : 252;
    memcpy(buffer, printed, copy_len);
    buffer[copy_len] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}