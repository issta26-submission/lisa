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
//<ID> 217
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num_node = cJSON_CreateNumber(1.5);
    cJSON *name_node = cJSON_CreateString("example");
    int buf_len = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    double before_value = 0.0;
    double set_ret = 0.0;
    double after_value = 0.0;
    cJSON_bool printed_ok = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "value", num_node);
    cJSON_AddItemToObject(root, "name", name_node);

    // step 3: Operate / Validate
    before_value = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "value"));
    set_ret = cJSON_SetNumberHelper(num_node, 42.42);
    after_value = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "value"));
    printed_ok = cJSON_PrintPreallocated(root, buffer, buf_len, 1);
    (void)before_value;
    (void)set_ret;
    (void)after_value;
    (void)printed_ok;
    (void)buffer[0];

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}