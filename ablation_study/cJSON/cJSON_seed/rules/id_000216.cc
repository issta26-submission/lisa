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
//<ID> 216
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num = cJSON_CreateNumber(0.0);
    cJSON *label = cJSON_CreateString("example");
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    double set_ret = 0.0;
    cJSON_bool printed_ok = 0;
    cJSON *retrieved = NULL;
    double retrieved_value = 0.0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "value", num);
    cJSON_AddItemToObject(root, "label", label);

    // step 3: Operate / Validate
    set_ret = cJSON_SetNumberHelper(num, 3.14159);
    retrieved = cJSON_GetObjectItem(root, "value");
    retrieved_value = cJSON_GetNumberValue(retrieved);
    printed_ok = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    (void)set_ret;
    (void)retrieved_value;
    (void)printed_ok;
    (void)buffer;

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}