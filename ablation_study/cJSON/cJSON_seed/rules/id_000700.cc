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
//<ID> 700
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *scratch = (char *)cJSON_malloc(256);
    memset(scratch, 0, 256);
    char *printed = NULL;
    cJSON *root = cJSON_CreateObject();
    cJSON *obj1 = cJSON_CreateObject();
    cJSON *obj2 = cJSON_CreateObject();
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_bool is_true = 0;
    cJSON_bool compare_result = 0;

    // step 2: Setup / Configure
    cJSON_AddNumberToObject(obj1, "n", 123.0);
    cJSON_AddFalseToObject(obj1, "f");
    cJSON_AddNumberToObject(obj2, "n", 123.0);
    cJSON_AddFalseToObject(obj2, "f");
    cJSON_AddItemToObject(root, "obj1", obj1);
    cJSON_AddItemToObject(root, "obj2", obj2);
    cJSON_AddItemToObject(root, "t", true_item);

    // step 3: Operate / Validate
    is_true = cJSON_IsTrue(true_item);
    cJSON_AddNumberToObject(root, "t_as_number", (double)is_true);
    compare_result = cJSON_Compare(obj1, obj2, 1);
    cJSON_AddNumberToObject(root, "compare_equal", (double)compare_result);
    printed = cJSON_PrintUnformatted(root);
    memcpy(scratch, printed ? printed : "", printed ? strlen(printed) : 0);
    if (printed) { cJSON_free(printed); printed = NULL; }  // single-line cleanup allowed (no branching in logic)

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}