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
//<ID> 17
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char json[] = "  [ \"keep\" , \"toDetach\" , 123 ]  ";
    cJSON *root = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    cJSON *ref_obj = (cJSON *)0;
    char *new_value = (char *)0;

    // step 2: Setup
    cJSON_Minify(json);
    root = cJSON_Parse(json);
    ref_obj = cJSON_CreateObject();

    // step 3: Operate
    detached = cJSON_DetachItemFromArray(root, 1);
    new_value = cJSON_SetValuestring(detached, "updated");
    cJSON_AddItemReferenceToObject(ref_obj, "myref", detached);

    // step 4: Validate
    cJSON_DeleteItemFromObject(ref_obj, "myref");

    // step 5: Cleanup
    cJSON_Delete(detached);
    cJSON_Delete(root);
    cJSON_Delete(ref_obj);

    // API sequence test completed successfully
    return 66;
}