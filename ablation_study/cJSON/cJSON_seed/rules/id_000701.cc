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
//<ID> 701
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *scratch = (char *)cJSON_malloc(128);
    memset(scratch, 0, 128);
    cJSON *root = cJSON_CreateObject();
    cJSON *other = cJSON_CreateObject();
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *other_true = cJSON_CreateTrue();
    cJSON *num_item = NULL;
    cJSON *found_true = NULL;
    cJSON_bool eq = 0;
    cJSON_bool is_true = 0;
    double val = 0.0;

    // step 2: Setup / Configure
    cJSON_AddNumberToObject(root, "num", 123.0);
    cJSON_AddFalseToObject(root, "flag_false");
    cJSON_AddItemToObject(root, "flag_true", true_item);
    cJSON_AddNumberToObject(other, "num", 123.0);
    cJSON_AddFalseToObject(other, "flag_false");
    cJSON_AddItemToObject(other, "flag_true", other_true);

    // step 3: Operate / Validate
    eq = cJSON_Compare(root, other, 1);
    found_true = cJSON_GetObjectItem(root, "flag_true");
    is_true = cJSON_IsTrue(found_true);
    num_item = cJSON_GetObjectItem(root, "num");
    val = cJSON_GetNumberValue(num_item);
    scratch[0] = eq ? '1' : '0';
    scratch[1] = is_true ? '1' : '0';
    scratch[2] = (char)('0' + ((int)val % 10));
    scratch[3] = '\0';

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);
    cJSON_Delete(other);

    // API sequence test completed successfully
    return 66;
}