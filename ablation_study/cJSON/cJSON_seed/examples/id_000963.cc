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
//<ID> 963
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_cJSON_api_sequence() {
    cJSON *root = cJSON_CreateObject();
    cJSON *node = cJSON_CreateString("initial");
    cJSON_AddItemToObject(root, "node", node);

    char *old_value = cJSON_SetValuestring(node, "updated");
    (void)old_value; /* do not free here to avoid double-free; root will own the string */

    cJSON *copied = cJSON_CreateString(cJSON_GetStringValue(node));
    cJSON_AddItemToObject(root, "copied", copied);

    cJSON *flag = cJSON_AddFalseToObject(root, "is_valid");

    char *old_copied = cJSON_SetValuestring(copied, "final");
    (void)old_copied; /* do not free here to avoid double-free; copied is owned by root */

    const char *verify = cJSON_GetStringValue(copied);
    (void)verify;
    (void)flag;

    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    cJSON_Delete(root);
    return 66;
}