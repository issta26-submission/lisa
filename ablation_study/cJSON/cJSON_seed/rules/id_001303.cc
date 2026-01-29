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
//<ID> 1303
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *first_item = NULL;
    cJSON *detached_item = NULL;
    char *printed = NULL;
    cJSON_bool is_true = 0;
    const char *json = "[ true, 123.0, false ]";

    // step 2: Setup / Configure
    root = cJSON_Parse(json);
    first_item = cJSON_GetArrayItem(root, 0);

    // step 3: Operate / Validate
    detached_item = cJSON_DetachItemViaPointer(root, first_item);
    is_true = cJSON_IsTrue(detached_item);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    (void)is_true;
    cJSON_free(printed);
    cJSON_Delete(root);
    cJSON_Delete(detached_item);
    // API sequence test completed successfully
    return 66;
}