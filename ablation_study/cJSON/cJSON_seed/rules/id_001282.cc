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
//<ID> 1282
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *num_item = NULL;
    cJSON *num_ref = NULL;
    cJSON *tmp = NULL;
    char *printed = NULL;
    double val_num = 0.0;
    double val_ref = 0.0;
    cJSON_bool add_ref_ok = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(3.1415);
    cJSON_AddItemToObject(root, "pi", num_item);
    num_ref = cJSON_CreateNumber(42.0);
    add_ref_ok = cJSON_AddItemReferenceToObject(root, "answer_ref", num_ref);
    cJSON_AddNullToObject(root, "nothing_here");

    // step 3: Operate / Validate
    tmp = cJSON_GetObjectItem(root, "pi");
    val_num = cJSON_GetNumberValue(tmp);
    tmp = cJSON_GetObjectItem(root, "answer_ref");
    val_ref = cJSON_GetNumberValue(tmp);
    printed = cJSON_PrintUnformatted(root);
    (void)val_num;
    (void)val_ref;
    (void)add_ref_ok;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    cJSON_Delete(num_ref);
    // API sequence test completed successfully
    return 66;
}