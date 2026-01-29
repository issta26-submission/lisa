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
//<ID> 1301
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *elem0 = NULL;
    cJSON *elem1 = NULL;
    cJSON *elem2 = NULL;
    cJSON *inner = NULL;
    cJSON *detached = NULL;
    char *printed = NULL;
    double detached_number = 0.0;
    cJSON_bool is_first_true = 0;
    const char *json = "[ true, 42.5, {\"inner\":true} ]";

    // step 2: Setup / Configure
    root = cJSON_Parse(json);
    elem0 = cJSON_GetArrayItem(root, 0);
    elem1 = cJSON_GetArrayItem(root, 1);
    elem2 = cJSON_GetArrayItem(root, 2);
    inner = cJSON_GetObjectItem(elem2, "inner");

    // step 3: Operate / Validate
    is_first_true = cJSON_IsTrue(elem0);
    detached = cJSON_DetachItemViaPointer(root, elem1);
    detached_number = cJSON_GetNumberValue(detached);
    printed = cJSON_PrintUnformatted(root);
    (void)is_first_true;
    (void)detached_number;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    cJSON_Delete(detached);
    // API sequence test completed successfully
    return 66;
}